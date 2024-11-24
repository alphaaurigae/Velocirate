# Velocirate

+ As of now the program downloads, extracts header for date and parses the json to a sqlite database.

- Current input ... CIK identifier record [https://www.sec.gov/files/company_tickers_exchange.json]
- Edgar Developer Resources  [ https://www.sec.gov/about/developer-resources]

## Build:

- Build OS: "Ubuntu 24.04"
- Cmake default, make may work but using cmake for testing etc.
- See deps file in proj root...

- ```./build_cmake.sh``` - build
- ```./clean_cmake.sh``` - remove build and start over. (does not remove db and input-file
- ```./test.sh``` - run program to fetch json if not exist / outdated and create db if not exist / outdated.

- Check cmakelist for build switches and options.

## Run:
- ```./test.sh``` - run program to fetch json if not exist / outdated and create db if not exist / outdated.

- By default the database is build in ```db/``` and ```input/``` for input json download.


## Flow:
1. Check if db exists, if not create db and create tables for headers and CIK if not exist.
2. Fetch CIK json and perform a backup for the old .json input file as well as delete previous backup. If out of date (compared to http header last modified and deb last modified entry on header table on the db) (the value is fetched from the file page as the json does not carry any meta values.)
3. Parse .json to db if 2 applied


## Work in progress... Todo e.g:
- Optimize json file / json file back creation / naming.
- RAII - classes.
- Expand to process company details, filings and calculate statistics for a up to date db with sanitized & ordered data.
- ERR # -fsanitize=thread  -fsanitize=leak -fsanitize=pointer-compare  -fsanitize=address 



