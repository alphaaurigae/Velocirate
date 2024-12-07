# Velocirate

> Program downloads, extracts header for date and parses the json to sqlite.

- Current input ... CIK identifier record [https://www.sec.gov/files/company_tickers_exchange.json]
- Edgar Developer Resources  [ https://www.sec.gov/about/developer-resources]


## Build

- Build OS: "Ubuntu 24.04"

... cmake ...

- ```./build_cmake.sh``` - build
- ```./clean_cmake.sh``` - tidy build && dirs
- ```./test.sh``` - 1. Create default db and input dir. 2. Run program to fetch json if not exist/dated. 3. Create/update db.


## Deps
```
- libssl-dev
- libcpprest-dev | https://github.com/Microsoft/cpprestsdk
- libsqlite3-dev
- sqlite3
- libboost-all-dev
- nlohmann-json3-dev | https://github.com/nlohmann/json
- zlib1g-dev | https://www.zlib.net
- libpthread-stubs0-dev
- libcrypto++-dev | https://github.com/weidai11/cryptopp
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
$ '/home/mmmm/Desktop/Velocirate/test.sh' 
Database opened successfully!
headers table created/verified successfully.
company_tickers table created/verified successfully.
File saved to: input//company_tickers_exchange.json
9981 rows successfully inserted.
```


## Flow:

1. Check if db exists, if not create db and create tables for headers and CIK if not exist.
2. Fetch CIK json and perform a backup for the old .json input file as well as delete previous backup. If out of date (compared to http header last modified and deb last modified entry on header table on the db) (the value is fetched from the file page as the json does not carry any meta values.)
3. Parse .json to db if 2 applied


## Todo e.g:

- Optimize json file / json file back creation / naming.
- RAII - classes.
- Expand to process company details, filings and calculate statistics for a up to date db with sanitized & ordered data.
- libcpprest ?

