updatePartitionTable() {
  local device="${1}"
  shift 1

  parted -s "${device}" "${@}"
}

makePartitionTable() {
  local device="${1}"
  local type="${2}"

  updatePartitionTable "${device}" mktable "${type}"
  addPartition_count=0
  addPartition_end=1
}

addPartition() {
  local device="${1}"
  local size="${2}"
  local fs="${3}"

  local number="${addPartition_count}"
  addPartition_count=$((addPartition_count + 1))
  local type="primary"

  local start="${addPartition_end}"
  local end

  if [ "${start}" -eq 0 ]
  then
    start="1s"
  else
    start="${start}MB"
  fi

  if [ -n "${size}" ]
  then
    addPartition_end=$((addPartition_end + size))
    end="${addPartition_end}MB"
  else
    end="100%"
    type="extended"
  fi

  updatePartitionTable "${device}" mkpart "${type}" "${fs}" "${start}" "${end}"
}

setPartitionProperty() {
  local device="${1}"
  local partition="${2}"
  local property="${3}"
  local state="${4}"

  updatePartitionTable "${device}" set "${partition}" "${property}" "${state}"
}

getPartitionIdentifier() {
  local device="${1}"
  local partition="${2}"

  sfdisk -c "${device}" "${partition}"
}

setPartitionIdentifier() {
  local device="${1}"
  local partition="${2}"
  local identifier="${3}"

  sfdisk -c "${device}" "${partition}" "${identifier}"
}

getPartitionLabel_Linux() {
  local device="${1}"

  e2fslabel "${device}"
}

makeFileSystem_Linux() {
  local device="${1}"
  local partition="${2}"
  local label="${3}"

  mke2fs -q -L "${label}" "${device}${partition}"
}

getPartitionLabel_DOS() {
  local device="${1}"

  dosfslabel "${device}"
}

makeFileSystem_DOS() {
  local device="${1}"
  local partition="${2}"
  local label="${3}"

  mkdosfs -n "${label}" "${device}${partition}"
}

verifyPartitions() {
  local device="${1}"
  shift 1

  local partition=0

  while [ "${#}" -ge 2 ]
  do
    partition=$((partition + 1))
    local subdevice="${device}${partition}"

    local requiredIdentifier="${1}"
    local requiredLabel="${2}"
    shift 2

    local actualIdentifier="$(getPartitionIdentifier "${device}" "${partition}" 2>/dev/null)"
    [ "${actualIdentifier}" = "${requiredIdentifier}" ] || return 1

    case "${actualIdentifier}"
    in
      [146bce]|1[146bce]) local actualLabel="$(getPartitionLabel_DOS "${subdevice}")";;
      83) local actualLabel="$(getPartitionLabel_Linux "${subdevice}")";;
      *) internalError "unsupported partition identifier: ${actualIdentifier}"
    esac

    [ "${actualLabel}" = "${requiredLabel}" ] || return 1
  done
}

isFileSystem() {
  local device="${1}"

  awk -v "device=${device}" '
    $1 == device {
      exit(1)
    }
  ' /proc/mounts || return 0

  return 1
}

isSwapSpace() {
  local device="${1}"

  awk -v "device=${device}" '
    $1 == device {
      exit(1)
    }
  ' /proc/swaps || return 0

  return 1
}

isActiveDevice() {
  local device="${1}"

  isFileSystem "${device}" && return 0
  isSwapSpace "${device}" && return 0
  return 1
}

