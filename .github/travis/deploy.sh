#!/usr/bin/env bash

set -Eeuxo pipefail

git push https://${GH_TOKEN}@github.com/argcv/argcv.git master --force

cd /tmp
git clone https://github.com/argcv/go-argcvapis.git
cd go-argcvapis
git commit --allow-empty -m "Upstream is updated"
git push https://${GH_TOKEN}@github.com/argcv/go-argcvapis.git master
