from sys import argv, exit
import os

MAX_THREADS = 32

def compile(codedir, flag):
    if flag == 'u':
          codefile = codedir + '_unoptimized'
    elif flag == 'o':
        codefile = codedir + '_optimized'
    else:
         print('Command line format:\n\t[Program, u for unoptimized, o for optimized]')
         exit(1)
    os.system(f'xlc -fopenmp -o {codedir}/{codefile}.o {codedir}/{codefile}.c')
    return codefile

def make_script(program_dir, codefile):
        M = int(MAX_THREADS/8) + 1
        with open(f"{program_dir}/ompjob.lsf", "w") as f:
            header = 'source /polusfs/setenv/setup.SMPI\n' + \
                f'#BSUB -n 1 -q normal\n' + \
                '#BSUB -W 03:00\n' + \
                f'#BSUB -o {codefile}.out\n' + \
                f'#BSUB -e {codefile}.err\n\n'
            
            f.write(header)

            i = 1
            while i < MAX_THREADS+1:
                #f.write(f'echo "============ THREADS: {i} ============""\n')
                omp_str = f'OMP_NUM_THREADS={i} mpiexec ./{program_dir}/{codefile}.o\n'
                i *= 2
                f.write(omp_str)

def run_script(codefile):
    program_dir = codefile.split("_")[0]
    os.system(f'bsub < {program_dir}/ompjob.lsf')

if __name__ == "__main__":
    if len(argv) == 3:
        program_dir = str(argv[1])
        codefile = compile(program_dir, argv[2])
        make_script(program_dir, codefile)
        run_script(codefile)
    else:
        print('Command line format:\n\t[Program, u for unoptimized, o for optimized]')
        