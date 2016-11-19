replicateString() {
  local variable="${1}"
  local string="${2}"
  local count="${3}"

  local length=$((${#string} * count))
  local result=""

  while [ "${#result}" -lt "${length}" ]
  do
    result="${result}${string}"
  done

  setVariable "${variable}" "${result}"
}

centerString() {
  local variable="${1}"
  local width="${2}"

  local string
  getVariable string "${variable}"
  local length="${#string}"

  if [ "${length}" -lt "${width}" ]
  then
    local count=$(((width - length) / 2))

    while [ "${count}" -gt 0 ]
    do
      string=" ${string}"
      count=$((count - 1))
    done

    while [ "${#string}" -lt "${width}" ]
    do
      string="${string} "
    done
  else
    local count=$(((length - width) / 2))

    while [ "${count}" -gt 0 ]
    do
      string="${string#?}"
      count=$((count - 1))
    done

    while [ "${#string}" -gt "${width}" ]
    do
      string="${string%?}"
    done
  fi

  setVariable "${variable}" "${string}"
}

