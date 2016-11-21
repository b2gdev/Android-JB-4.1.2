replicateString() {
  local replicateString_variable="${1}"
  local replicateString_string="${2}"
  local replicateString_count="${3}"

  local replicateString_length=$((${#replicateString_string} * replicateString_count))
  local replicateString_result=""

  while [ "${#replicateString_result}" -lt "${replicateString_length}" ]
  do
    replicateString_result="${replicateString_result}${replicateString_string}"
  done

  setVariable "${replicateString_variable}" "${replicateString_result}"
}

trimLeft() {
  local trimLeft_variable="${1}"

  local trimLeft_string
  getVariable trimLeft_string "${trimLeft_variable}"
  local trimLeft_changed=false

  while true
  do
    local trimLeft_trimmed="${trimLeft_string# }"
    [ "${trimLeft_trimmed}" = "${trimLeft_string}" ] && break

    trimLeft_string="${trimLeft_trimmed}"
    trimLeft_changed=true
  done

  ! "${trimLeft_changed}" || setVariable "${trimLeft_variable}" "${trimLeft_string}"
}

trimRight() {
  local trimRight_variable="${1}"

  local trimRight_string
  getVariable trimRight_string "${trimRight_variable}"
  local trimRight_changed=false

  while true
  do
    local trimRight_trimmed="${trimRight_string% }"
    [ "${trimRight_trimmed}" = "${trimRight_string}" ] && break

    trimRight_string="${trimRight_trimmed}"
    trimRight_changed=true
  done

  ! "${trimRight_changed}" || setVariable "${trimRight_variable}" "${trimRight_string}"
}

trimString() {
  local trimString_variable="${1}"

  trimLeft "${trimString_variable}"
  trimRight "${trimString_variable}"
}

centerString() {
  local centerString_variable="${1}"
  local centerString_width="${2}"

  local centerString_string
  getVariable centerString_string "${centerString_variable}"
  trimString centerString_string
  local centerString_length="${#centerString_string}"

  if [ "${centerString_length}" -lt "${centerString_width}" ]
  then
    local centerString_count=$(((centerString_width - centerString_length) / 2))

    while [ "${centerString_count}" -gt 0 ]
    do
      centerString_string=" ${centerString_string}"
      centerString_count=$((centerString_count - 1))
    done

    while [ "${#centerString_string}" -lt "${centerString_width}" ]
    do
      centerString_string="${centerString_string} "
    done
  else
    local centerString_count=$(((centerString_length - centerString_width) / 2))

    while [ "${centerString_count}" -gt 0 ]
    do
      centerString_string="${centerString_string#?}"
      centerString_count=$((centerString_count - 1))
    done

    while [ "${#centerString_string}" -gt "${centerString_width}" ]
    do
      centerString_string="${centerString_string%?}"
    done
  fi

  setVariable "${centerString_variable}" "${centerString_string}"
}

