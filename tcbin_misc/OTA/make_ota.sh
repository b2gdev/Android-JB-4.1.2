#!/bin/sh
if [ $# -lt 1 ]; then
		echo "usage $ make_ota [output name]\n"
        exit
fi
cd update
zip -9r ../unsigned.zip * -x system/.gitignore
cd -
java -jar signapk.jar -w release.x509.pem release.pk8 unsigned.zip $1
