#!/bin/sh
echo -ne '\033c\033]0;demo\a'
base_path="$(dirname "$(realpath "$0")")"
"$base_path/demo.x86_64" "$@"
