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
    programMessage "build directory: ${buildDirectory}"
  elif testBuildDirectory "${directory}"
  then
    buildDirectory="${directory}"
  else
    semanticError "not a build tree: ${directory}"
  fi

  cd "${buildDirectory}"
  readonly buildDirectory="$(pwd)"
  export GIT_WORK_TREE="${buildDirectory}"
  export GIT_DIR="${GIT_WORK_TREE}/.git"
}

showBuildIdentifier() {
  git "--work-tree=${programDirectory}" describe --tags --always --abbrev=1 --dirty=MODIFIED
}

