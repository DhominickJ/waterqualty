server-db-install:
	winget install --id MongoDB.Server
	winget install --id MongoDB.Compass.Community
db:
	mongod --port 6969 --dbpath ./database/
run_server:
	node .
dependencies:
	npm install
