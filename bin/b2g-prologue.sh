set -e
umask 022

readonly scriptName="${0##*/}"
readonly programName="${scriptName%.sh}"

programDirectory="${0%/*}"
[ -n "${programDirectory}" ] || programDirectory="."
readonly programDirectory="$(cd "${programDirectory}" && pwd)"

readonly logFile="${programName}.log"
readonly pidFile="${programName}.pid"

readonly targetProduct="beagleboard"
readonly targetArchitecture="arm"
readonly toolPrefix="arm-eabi-"

readonly loaderDirectory="x-loader"
readonly bootDirectory="u-boot"
readonly kernelDirectory="kernel"
readonly scriptsDirectory="tcbin_misc/build_scripts"
readonly imageDirectory="image_folder"

readonly bootImageDirectory="${imageDirectory}/Boot_Images"
readonly loaderImage="${bootImageDirectory}/MLO"
readonly bootImage="${bootImageDirectory}/u-boot.bin"
readonly modulesImage="${bootImageDirectory}/uImage"

readonly nandImageDirectory="${imageDirectory}/NandFS"
readonly androidImage="${nandImageDirectory}/norm_uImage"
readonly systemImage="${nandImageDirectory}/ubi_system.img"
readonly recoveryImage="${nandImageDirectory}/bk_uImage"

readonly loaderName="X-Loader"
readonly bootName="U-Boot"
readonly androidName="Android"
readonly recoveryName="recovery"

unset MAKEFLAGS=

writeLine() {
  local line="${1}"

  echo "${line}"
}

logLine() {
  local line="${1}"

  writeLine "${line}"
}

programMessage() {
  local message="${1}"

  echo >&2 "${programName}: ${message}"
}

syntaxError() {
  local message="${1}"

  programMessage "${message}"
  exit 2
}

semanticError() {
  local message="${1}"

  programMessage "${message}"
  exit 3
}

internalError() {
  local message="${1}"

  programMessage "${message}"
  exit 4
}

mutuallyExclusiveOptions() {
  local options=""

  while [ "${#}" -ge 2 ]
  do
    "${2}" && {
      [ -z "${options}" ] || options="${options} "
      options="${options}-${1}"
    }

    shift 2
  done

  [ "${options}" = "${options% *}" ] || syntaxError "mutually exclusive options: ${options}"
}

showHelp() {
cat <<END_HELP
usage: ${scriptName} [option ...]
END_HELP
}

handleOption_h() {
  local returnCode=0
  showHelp || returnCode="${?}"
  exit "${returnCode}"
}

noMoreParameters() {
  [ "${#}" -eq 0 ] || syntaxError "too many parameters: ${*}"
}

handleParameters() {
  noMoreParameters "${@}"
}

handleArguments() {
  local options="${1}"
  shift 1

  local option
  while getopts ":${options}" option
  do
    case "${option}"
    in
      :) syntaxError "missing operand: -${OPTARG}";;
     \?) syntaxError "unrecognized option: -${OPTARG}";;
      *) "handleOption_${option}";;
    esac
  done

  shift $((OPTIND - 1))
  handleParameters "${@}"
}

addCleanupCommand() {
  local command="${1}"

  cleanupCommands="${command} ${cleanupCommands}"
}

executeCleanupCommands() {
  set +e

  [ -z "${cleanupCommands}" ] || {
    local command

    for command in ${cleanupCommands}
    do
      "${command}"
    done
  }
}

cleanupCommands=""
trap executeCleanupCommands exit

removeTemporaryDirectory() {
  rm -f -r "${temporaryDirectory}"
}

needTemporaryDirectory() {
  readonly temporaryDirectory="$(mktemp -d --tmpdir "${programName}.${$}.XXXXXX")"
  addCleanupCommand removeTemporaryDirectory
}

lockScriptExecution() {
  exec 9>>"${pidFile}"
  flock -x -n 9 || semanticError "build already running: ${rootDirectory}"
  addCleanupCommand unlockScriptExecution
  echo "${$}" >"${pidFile}"
}

unlockScriptExecution() {
  echo -n "" >"${pidFile}"
  flock -u 9
  exec 9>&-
}

startLogFile() {
  logLine() {
    local line="${1}"

    writeLine "${programName}: $(date "+%Y-%m-%d@%H:%M:%S") ${line}"
  }

  exec >"${logFile}" 2>&1
}

testBuildDirectory() {
  local directory="${1}"

  [ -f "${directory}/Makefile" ] || return 1
  [ -d "${directory}/${loaderDirectory}" ] || return 1
  [ -d "${directory}/${bootDirectory}" ] || return 1
  [ -d "${directory}/${kernelDirectory}" ] || return 1
  [ -d "${directory}/${scriptsDirectory}" ] || return 1
}

setBuildDirectory() {
  local directory="${1}"

  if [ -z "${directory}" ]
  then
    directory="${programDirectory}"
    local currentDirectory="${directory}"

    while true
    do
      [ -z "${currentDirectory}" ] && semanticError "build tree not found: ${directory}"
      testBuildDirectory "${currentDirectory}" && break
      currentDirectory="${currentDirectory%/*}"
    done

    buildDirectory="${currentDirectory}"
  elif testBuildDirectory "${directory}"
  then
    buildDirectory="${directory}"
  else
    semanticError "not a build tree: ${directory}"
  fi

  cd "${buildDirectory}"
  readonly buildDirectory="$(pwd)"
}

