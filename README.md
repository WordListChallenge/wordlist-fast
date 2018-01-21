#### Building and running

* Install the dependencies (ubuntu: `build-essential`, `cmake`, `pkg-config`, `libboost-all-dev`)
* Change to the root of the source code directory and build:

        cd wordlist-fast
        cmake . && make


* The resulting executable can be invoked as follows:

        time ./wordlist testdata/wordlist_utf8.txt output.txt
