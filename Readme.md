# Velocirate

> Program downloads, extracts header for date and parses the json to sqlite.

- Current input ... CIK identifier record [https://www.sec.gov/files/company_tickers_exchange.json]
- Edgar Developer Resources  [ https://www.sec.gov/about/developer-resources]


## Build

- Build OS: "Ubuntu 24.04"

... cmake ...
(g++/clang - Cmakelists "option(USE_CLANG" ON/OFF - default g++ (clang complains! See "Todo" below)) 

- ```./build_cmake.sh``` - build
- ```./clean_cmake.sh``` - tidy build && dirs
- ```./test.sh``` - 1. Create default db and input dir. 2. Run program to fetch json if not exist/dated. 3. Create/update db.


## Deps
```
- libpoco-dev* | https://pocoproject.org/ | https://github.com/pocoproject/poco
- libssl-dev
- libsqlite3-dev
- sqlite3


```


## Helpful Tools
```
- sqlitebrowser |  https://github.com/sqlitebrowser/sqlitebrowser
```

## Run:
```
$ bin/velocirate -h
Error: Unknown option -h
Usage: 
  --inputpath, -ip <path>  Specify the input path for saving the JSON file
  --database, -db <path>   Specify the database directory path
```
```
$ '/home/mmmm/Desktop/proj/git/Velocirate/test.sh' 
mkdir: cannot create directory ‘input’: File exists
mkdir: cannot create directory ‘db’: File exists
bin/velocirate --inputpath input/ --database db/
Database opened successfully!
headers table created/verified successfully.
company_tickers table created/verified successfully.
HTTP Status: 200
File saved to: input//company_tickers_exchange.json
10038 rows successfully inserted.
total 1184
drwxrwxr-x 2 mmmm mmmm    4096 Jan  5 12:07 .
drwxrwxr-x 8 mmmm mmmm    4096 Jan  5 12:07 ..
-rw-rw-r-- 1 mmmm mmmm 1201252 Jan  5 12:07 company_tickers_exchange.json
total 472
drwxrwxr-x 2 mmmm mmmm   4096 Jan  5 12:07 .
drwxrwxr-x 8 mmmm mmmm   4096 Jan  5 12:07 ..
-rw-r--r-- 1 mmmm mmmm 475136 Jan  5 12:07 data.db

```


## Flow:

1. Check if db exists, if not create db and create tables for headers and CIK if not exist.
2. Fetch CIK json and perform a backup for the old .json input file as well as delete previous backup. If out of date (compared to http header last modified and deb last modified entry on header table on the db) (the value is fetched from the file page as the json does not carry any meta values.)
3. Parse .json to db if 2 applied


## Todo

- Optimize json file / json file back creation / naming.
- RAII - classes.
- Expand to process company details, filings and calculate statistics for a up to date db with sanitized & ordered data.
- libcpprest ?
