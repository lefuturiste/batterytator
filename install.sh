user=$(cat ./tmp_install_user.txt)

echo "> Using user: ${user}"

echo "> Using battery: ${BAT_ID}"

# install the binary
cp ./batterytator /usr/bin/batterytator

# install the service unit file
unitPath=/usr/lib/systemd/system/batterytator.service

cp ./batterytator.service ${unitPath}.tmp

env BATTERYTATOR_USER=${user} envsubst < ${unitPath}.tmp > ${unitPath}

rm ${unitPath}.tmp

# replace the user and group in the service unit file
sed -i 's/REPLACE_USER/'${user}'/g' ${unitPath}
sed -i 's/REPLACE_GROUP/'${user}'/g' ${unitPath}

# replace battery ID
sed -i 's/REPLACE_BATTERY_ID/'${BAT_ID}'/g' ${unitPath}

# clean the temporary file
rm ./tmp_install_user.txt

echo "> By default the systemd service is not started. But if you want you can type :"

echo "systemctl enable batterytator"
echo "systemctl start batterytator"

echo "I don't recommand systemd though, instead use your ~/.xinitrc"

echo "> The installation script of batterytator was executed. Thanks!"
