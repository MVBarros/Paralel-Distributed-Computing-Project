queries=("20-1000000-0.query" "3-20000000-0.query" "4-10000000-0.query" "4-20000000-0.query" "3-5000000-0.query")

for n in 1 2 4 8 16 32 64
do
    for query in "${queries[@]}"
    do
        diff ---ignore-all-space queries/expected/${query} queries/obtained/${n}/${query}
    done
done