from tabulate import tabulate

def read_time(path: str) -> str:
    with open(path, 'r') as f:
        return f.read()

trees = ["20 1000000 0", "3 5000000 0", "4 10000000 0", "3 20000000 0", "4 20000000 0"]

process_counts = ["1", "2", "4", "8", "16", "32", "64"]

table = [[tree] for tree in trees]

for line, tree in zip(table, trees):
    tree_file = tree.replace(" ", "-") + ".time"
    for count in process_counts:
        path = f"exec_times/{count}/{tree_file}"
        line.append(read_time(path))

headers = ["Arguments", *(f"{count}" for count in process_counts)]

print(tabulate(table, headers=headers, tablefmt="github"))