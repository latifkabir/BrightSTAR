STARHOME="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
export STARHOME

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$STARHOME/lib:$STARHOME/analysis/lib