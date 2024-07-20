mkdir -p build
if [[ "$*" == *"-clean"* ]]
then
	echo ".......---- Cleaning current files ---......."
	cd build
	rm -rf *
	cd ..
fi

if [[ "$*" == *"-build"* ]]
then
	cd build
	echo ".......---- Building project ---......."
	sh ../build.sh
	echo ".......---- Build finished ---......."
	cd ..
fi

if [[ "$*" == *"-push"* ]]
then
	sshpass -p 1 sudo mkdir -p /opt/joint/
	cd build
	echo ".......---- Push to operation directory ---......."
	sshpass -p 1 sudo scp joint /opt/joint/
	cd ..
	sshpass -p 1 sudo scp -r ./config/ /opt/joint/
	echo ".......---- Pushing finished ---......."
	cd ..
fi