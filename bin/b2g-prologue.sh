set -e
umask 022

readonly scriptName="${0##*/}"
readonly programName="${scriptName%.sh}"

programDirectory="${0%/*}"
[ -n "${programDirectory}" ] || programDirectory="."
readonly programDirectory="$(cd "${programDirectory}" && pwd)"

readonly initialDirectory="$(pwd)"

readonly logFile="${programName}.log"
readonly pidFile="${programName}.pid"

getFormattedTime() {
  date "+%Y-%m-%d@%H:%M:%S"
}

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

includeShellDefinitions() {
  local name="${1}"

  . "${programDirectory}/${name}.sh"
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
  readonly temporaryDirectory="$(mktemp -d --tmpdir "${programName}.$(getFormattedTime).${$}.XXXXXX")"
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

    writeLine "${programName}: $(getFormattedTime) ${line}"
  }

  exec >"${logFile}"
  programMessage "log file: ${logFile}"
  exec 2>&1
}

