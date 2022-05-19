import pandas as pd
import re


def delete_unnecessary_lines():
    """
    Deleting all other lines, leaves only the last launch results in the file.
    """
    with open("result.out", "r+") as f:
        fpos, seek_pos = 0, 0
        for line in f:
            fpos += len(line)
            if "Successfully completed.\n" in line:
                seek_pos = fpos
        f.seek(seek_pos)
        necessary_lines = f.readlines()
        f.seek(0)
        f.truncate()
        for l in necessary_lines:
            f.write(l)

        f.seek(0)


def parse_results_from_file():
    """
    Extracts and reads the necessary lines in the results file to create a collection of dataframes combining them.
    Then returns this collection of dataframes, possibly passed to a function afterwards in order to print or write to another file of specialized type(Excel, CSV etc.)
    """
    N = []
    threads = []
    avg_spmv = []
    avg_axpby = []
    avg_ddot = []
    avg_iter = []
    total_iter = []
    cube_gen = []
    df_collection = []
    with open("result.out", "r") as f:
        for line in f:
            if 128 in threads and len(avg_ddot) == len(threads):
                df = pd.DataFrame(
                    {
                        "N_Size": N,
                        "Threads": threads,
                        "Cube_Gen": cube_gen,
                        "Total_Iter": total_iter,
                        "Avg_Iter": avg_iter,
                        "Avg_SpMV": avg_spmv,
                        "Avg_axpby": avg_axpby,
                        "Avg_ddot": avg_ddot,
                    }
                )
                df_collection.append(df)
                threads.clear()
                avg_spmv.clear()
                avg_axpby.clear()
                avg_ddot.clear()
                avg_iter.clear()
                cube_gen.clear()
                total_iter.clear()
                N.clear()

            find = re.compile(r".*?: (.*)s.*")
            if "OMP" in line:
                threads.append(int(re.findall("[0-9]+", line)[0]))
            if "Sparse Matrix Size" in line:
                N.append(re.findall("[0-9]+", line)[0])
            if "generation" in line:
                cube_gen.append(float(re.search(find, line).group(1)))
            if "Total time of all iterations" in line:
                total_iter.append(float(re.search(find, line).group(1)))
            if "Average time of an iteration" in line:
                avg_iter.append(float(re.search(find, line).group(1)))
            if "SpMV" in line:
                avg_spmv.append(float(re.search(find, line).group(1)))
            if "axpby" in line:
                avg_axpby.append(float(re.search(find, line).group(1)))
            if "ddot" in line:
                avg_ddot.append(float(re.search(find, line).group(1)))
    return df_collection


def write_data_to_excel(df_collection):
    """
    Creates an excel file and writes the data from the array of dataframe to different sheets in it.
    """
    with pd.ExcelWriter("data.xlsx") as writer:
        for i in range(len(df_collection)):
            df_collection[i].to_excel(
                writer, sheet_name=df_collection[i].N_Size[0], index=False
            )


if __name__ == "__main__":
    # delete_unnecessary_lines()
    df_collection = parse_results_from_file()
    write_data_to_excel(df_collection)
