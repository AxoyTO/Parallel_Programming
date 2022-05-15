import re
import pandas as pd


def parse_data(filename="result.out"):
    processes_list = []
    sizes_list = []
    T1_times_list = []
    T2_times_list = []
    total_times_list = []
    with open(filename, "r") as file:
        for line in file:
            if "Successfully completed." in line:
                break

        for line in file:
            if "MATRIX" in line and "echo" not in line:
                print(line)
                sizes_list.append(
                    re.search("\d+.x.\d+", line).group(0).replace(" ", "")
                )
            elif "PROCESSES" in line and "echo" not in line:
                print(line)
                processes_list.append(int(re.search("\d+", line).group(0)))
            elif re.match(r"(.*?)(T1\))", line):
                T1_times_list.append(float(re.search("(\d+.\d+).", line).group(0)))
            elif re.match(r"(.*?)(\(T2)", line):
                T2_times_list.append(float(re.search("(\d+.\d+).", line).group(0)))
            elif re.match(r"(.*?)(T1\+T2)", line):
                total_times_list.append(float(re.search("(\d+.\d+).", line).group(0)))

    sizes_list = [x for x in sizes_list for _ in range(len(set(processes_list)))]

    for i in range(len(sizes_list)):
        print(i, sizes_list[i], processes_list[i])

    print(len(sizes_list))
    print(len(processes_list))
    # print(sizes_list)

    data_per_frame = len(set(processes_list))
    data = []
    """
    for i in range(0, len(sizes_list), data_per_frame):
        data.append(
            pd.DataFrame(
                {
                    "MatrixSize": sizes_list[i : i + data_per_frame],
                    "Processes": processes_list[i : i + data_per_frame],
                    "T1": T1_times_list[i : i + data_per_frame],
                    "T2": T2_times_list[i : i + data_per_frame],
                    "Total": total_times_list[i : i + data_per_frame],
                }
            )
        )
    """
    # print(data)
    return data


def write_data_to_excel(data):
    with pd.ExcelWriter("data.xlsx") as writer:
        for i in range(len(data)):
            data[i].to_excel(writer, sheet_name=data[i].MatrixSize[0], index=False)


if __name__ == "__main__":
    data = parse_data()
    # write_data_to_excel(data)
