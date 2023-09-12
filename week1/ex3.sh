echo "Creating root folder..."
date
sleep 3
mkdir ./root


echo "Creating root file..."
date
sleep 3
ls -ltr / > "root/root.txt" 

echo "Creating home folder..."
date
sleep 3
mkdir ./home

echo "Creating home file..."
date
sleep 3
ls -ltr ~ > "home/home.txt" 


echo "Content of root.txt:"
cat root/root.txt

echo "Content of home.txt:"
cat home/home.txt

echo "Content of home directory:"
ls -ltr home

echo "Content of root directory:"
ls -ltr root
