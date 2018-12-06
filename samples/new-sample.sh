#!/bin/bash
#
# new-app.sh -- creates a new sub-project/app
#

# Record info about the shell script being run
SCRIPT_BASENAME="$(basename "${0}")"
SCRIPT_DIRNAME="$(dirname "${0}")"

# Argument parsing and check(s)
function print_usage {
    >&2 echo "Usage: ${SCRIPT_BASENAME} NEW_APP_NAME"
    >&2 echo ""
}
if [ "$#" -ne 1 ]; then
    >&2 echo "ERROR: Invalid number of arguments"
    >&2 echo ""
    print_usage
    exit 1
fi
NEW_APP_NAME="${1}"

# Do further operations from within the parent-directory to other samples
cd "${SCRIPT_DIRNAME}"

# Don't overwrite older app(s)
if [ -d "${NEW_APP_NAME}" ]; then
    >&2 echo "ERROR: An app with the given name (\"${NEW_APP_NAME}\") already exists!"
    >&2 echo ""
    exit 1
fi

# Create a directory for the sample, populating it with files
echo
echo "Copying:"
cp -av "template-sample" "${NEW_APP_NAME}"

# Rename file(s) as-needed
echo
echo "Renaming:"
for FILE in $(ls "${NEW_APP_NAME}"); do
    case "${FILE}" in
        *TEMPLATE*)
            FILE_RENAMED="${FILE/TEMPLATE/${NEW_APP_NAME}}"
            mv -fv "${NEW_APP_NAME}/${FILE}" "${NEW_APP_NAME}/${FILE_RENAMED}"
            ;;
        *)
            ;;
    esac
done

# Do further operations from within the new sample's directory

# Modify text-file content as-needed
echo
echo "Modifying:"
pushd "${NEW_APP_NAME}" > /dev/null
for FILE in $(ls); do
    if [[ "$(file "${FILE}")" == *"ASCII text"* ]]; then
        echo "${NEW_APP_NAME}/${FILE}"
        perl -p -i.bak -e "s/TEMPLATE/${NEW_APP_NAME}/g" "${FILE}"
    fi
done
popd > /dev/null

echo
echo "Cleaning-up / deleting: "
rm -fv "${NEW_APP_NAME}"/*.bak

echo
echo "DONE!"
