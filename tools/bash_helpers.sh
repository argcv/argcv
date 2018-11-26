tc_esc=$(printf "\e")
tc_black="$tc_esc[0;30m"
tc_red="$tc_esc[0;31m"
tc_green="$tc_esc[0;32m"
tc_orange="$tc_esc[0;33m"
tc_blue="$tc_esc[0;34m"
tc_purple="$tc_esc[0;35m"
tc_cyan="$tc_esc[0;36m"
tc_l_gray="$tc_esc[0;37m"

tc_d_gray="$tc_esc[1;30m"
tc_l_red="$tc_esc[1;31m"
tc_l_green="$tc_esc[1;32m"
tc_yellow="$tc_esc[1;33m"
tc_l_blue="$tc_esc[1;34m"
tc_l_purple="$tc_esc[1;35m"
tc_l_cyan="$tc_esc[1;36m"
tc_white="$tc_esc[1;37m"

tc_reset="$tc_esc[0m"

die() {
  # Print a message and exit with code 1.
  #
  # Usage: die <error_message>
  #   e.g., die "Something bad happened."

  echo -e $@
  exit 1
}

PLATFORM="$(uname -s | tr 'A-Z' 'a-z')"

function is_linux() {
  [[ "${PLATFORM}" == "linux" ]]
}

function is_macos() {
  [[ "${PLATFORM}" == "darwin" ]]
}

function is_windows() {
  # On windows, the shell script is actually running in msys
  [[ "${PLATFORM}" =~ msys_nt*|mingw*|cygwin*|uwin* ]]
}

function is_ppc64le() {
  [[ "$(uname -m)" == "ppc64le" ]]
}

function sed_in_place() {
  sed -e $1 $2 > "$2.bak"
  mv "$2.bak" $2
}

function write_to_bazelrc() {
  echo "$1" >> $BAZELRC
}

function write_action_env_to_bazelrc() {
  # echo "write_action_env_to_bazelrc/$1/$2"
  if [ -z $2 ]; then
    echo -e "[ENV] ${tc_cyan}UNSET$tc_reset $1"
    write_to_bazelrc "build --action_env=$1="
  else
    echo "[ENV] ${tc_green}SET$tc_reset: $1 \"$2\""
    write_to_bazelrc "build --action_env=$1=\"$2\""
  fi
}

#
function check_and_write_action_env_to_bazelrc() {
  write_action_env_to_bazelrc "$1" "${!1}"
}

function update_interactive_mode_status() {
  INTERACTIVE_MODE="1"

  if [[ ( $DEBIAN_FRONTEND == "noninteractive" ) || ( $DOCKER_FRONTEND == "noninteractive" ) ]]; then
    INTERACTIVE_MODE="0"
  fi

  local fd=0   # stdin, to test non-interactive

  if [[ ! -t "$fd" && ! -p /dev/stdin ]]; then
    INTERACTIVE_MODE="0"
  fi

}

[ -z $INTERACTIVE_MODE ] && update_interactive_mode_status

function is_non_interactive_mode() {
  [[ ${INTERACTIVE_MODE} == "0" ]]
}

function is_interactive_mode() {
  [[ ${INTERACTIVE_MODE} == "1" ]]
}

# check if one command is exists
# usage:
# if check_command_exists ssh; then
#   echo "ssh is exists"
# else
#   echo "command ssh not found"
# fi
function check_command_exists() {
  hash $1 2>/dev/null
}

function argcv_prompt_exitstatus()
{
  if [[ $? == 0 ]]; then
    echo $tc_green'🍺'"  +1s"$tc_reset
  else
    echo $tc_red'💔'"  -${?}s"$tc_reset
  fi
}

function argcv_prompt_check_root() {
  if [ $(id -u) -eq 0 ]; then
    echo "# "
  else
    echo "$ "
  fi
}


