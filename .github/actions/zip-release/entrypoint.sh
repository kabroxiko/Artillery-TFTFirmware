#!/bin/bash

set -ex

# Loop through each hardware entry in hardware.json
jq -c '.include[]' "hardware.json" | while read -r entry; do
  hardware=$(echo "$entry" | jq -r '.hardware')
  tftfolder=$(echo "$entry" | jq -r '.tftfolder')
  customlogo=$(echo "$entry" | jq -r '.customlogo')
  pwd
  ls -l firmwares
  ls -l firmwares/${hardware}

  mv "firmwares/${hardware}"/*.bin "MKS${tftfolder}.bin"
  zip -mur "${hardware}.zip" "MKS${tftfolder}.bin"
  cd "${GITHUB_WORKSPACE}/Copy to SD Card root directory to update"
  zip -ur ${GITHUB_WORKSPACE}/${hardware}.zip config.ini
  cd "${GITHUB_WORKSPACE}/Copy to SD Card root directory to update/THEME_Unified Menu Material theme"
  if [ "${tftfolder}" = "TFT32" ]; then
    cp -a "TFT28" "${tftfolder}"
  fi
  zip -ur "${GITHUB_WORKSPACE}/${hardware}.zip" "${tftfolder}"
  if [ "${customlogo}" != "null" ] && [ -d "${customlogo}" ]; then
    echo "Adding custom logo for ${hardware}"
    mkdir -p "${GITHUB_WORKSPACE}/firmwares/${hardware}/${tftfolder}/bmp"
    cp -a "${customlogo}/*" "${GITHUB_WORKSPACE}/firmwares/${hardware}/${tftfolder}/bmp/"
    cd "${GITHUB_WORKSPACE}/firmwares/${hardware}"
    zip -ur "${GITHUB_WORKSPACE}/${hardware}.zip" "${tftfolder}"
  fi
  cd ${GITHUB_WORKSPACE}
done

# Output zip list as a file and as a GitHub Actions output (if needed)
echo 'files<<EOF' >> $GITHUB_OUTPUT
ls -1 *.zip | tee -a $GITHUB_OUTPUT
echo 'EOF' >> $GITHUB_OUTPUT
