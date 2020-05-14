#!/usr/bin/env bash

set -e

source="${BASH_SOURCE[0]}"
while [[ -h "${source}" ]]; do
	dir="$(cd -P "$(dirname "${source}")" >/dev/null 2>&1 && pwd)"
	source="$(readlink "${source}")"
	[[ "${source}" != /* ]] && source="${dir}/${source}"
done
script_dir="$(cd -P "$(dirname "${source}")" >/dev/null 2>&1 && pwd)"
cd "${script_dir}"

source "./gdb-wrap.sh"