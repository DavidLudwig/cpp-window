#!/bin/bash
#
# rename-app.sh -- renames a sub-project/app
#

# Record info about the shell script being run
SCRIPT_BASENAME="$(basename "${0}")"
SCRIPT_DIRNAME="$(dirname "${0}")"

# Argument parsing and check(s)
function print_usage {
    >&2 echo "Usage: ${SCRIPT_BASENAME} OLD_APP_NAME NEW_APP_NAME"
    >&2 echo ""
}
if [ "$#" -ne 2 ]; then
    >&2 echo "ERROR: Invalid number of arguments"
    >&2 echo ""
    print_usage
    exit 1
fi
OLD_APP_NAME="${1}"
NEW_APP_NAME="${2}"

# Do further operations from within the parent-directory to other samples
cd "${SCRIPT_DIRNAME}"

# Make sure the 'old' app exists
if [ ! -d "${OLD_APP_NAME}" ]; then
    >&2 echo "ERROR: An app with the provided 'old' name (\"${OLD_APP_NAME}\") does not exist!"
    >&2 echo ""
    exit 1
fi

# Don't overwrite other app(s)
if [ -d "${NEW_APP_NAME}" ]; then
    >&2 echo "ERROR: An app with the provided 'new' name (\"${NEW_APP_NAME}\") already exists!"
    >&2 echo ""
    exit 1
fi

# Utility function(s)
function old_to_new_file_name {
    echo ${1} | sed "s/^${OLD_APP_NAME}/${NEW_APP_NAME}/g"
}

# Make sure we won't overwrite files, when renaming them
for OLD_FILE in $(ls "${OLD_APP_NAME}/"); do
    NEW_FILE=$(old_to_new_file_name "${OLD_FILE}")
    if [[ "${OLD_FILE}" != "${NEW_FILE}" ]]; then
        if [ -f "${OLD_APP_NAME}/${NEW_FILE}" ]; then
            >&2 echo "ERROR: A file in the app cannot be safely renamed, as it overwrite an existing file:"
            >&2 echo "                  old / file to rename: ${OLD_FILE}"
            >&2 echo "  new / file that would be overwritten: ${NEW_FILE}"
            exit 1
        fi
    fi
done

# Rename the app's main directory
echo
echo "Renaming directory:"
mv -v "${OLD_APP_NAME}" "${NEW_APP_NAME}"

# Rename individual file(s) as-needed
echo
echo "Renaming files:"
for OLD_FILE in $(ls "${NEW_APP_NAME}/"); do
    NEW_FILE=$(old_to_new_file_name "${OLD_FILE}")
    if [[ "${OLD_FILE}" != "${NEW_FILE}" ]]; then 
        mv -v "${NEW_APP_NAME}/${OLD_FILE}" "${NEW_APP_NAME}/${NEW_FILE}"
        perl -p -i.bak 
    fi
done

echo
echo "DONE!"
