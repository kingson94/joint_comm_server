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
	cd build
	echo ".......---- Push to operation directory ---......."
	scp joint /opt/joint/
	echo ".......---- Pushing finished ---......."
	cd ..
fi