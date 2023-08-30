#!/bin/bash

set -e
set -o pipefail

GIT=${GIT:="git"}

repoRoot="$("$GIT" rev-parse --show-toplevel)"
cd "$repoRoot"/tools/codegen || exit 255

[ -f "$repoRoot/include.orig" ] && chmod -R u+w "$repoRoot/include.orig"
rm -rf "$repoRoot"/include.orig/

cp -r "$repoRoot"/include/ "$repoRoot"/include.orig/

chmod -R u-w "$repoRoot/include.orig"

USER_SHELL=${USER_SHELL:="$(getent passwd | awk -F: -v user="$USER" '$1 == user {print $NF}')"}

echo "Make your change in include directory then exit shell"

cd "$repoRoot"

"$USER_SHELL"

! diff -ruN "include.orig/" "include/" >"$repoRoot"/tools/codegen/fix-unknow-types.patch

chmod -R u+w "$repoRoot/include.orig"
rm -rf "$repoRoot"/include.orig/
