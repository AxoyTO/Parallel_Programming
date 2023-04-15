from sys import argv, exit
import os

MAX_PROCESSES = 32

def compile(codedir, flag):
    #os.system(f'module load SpectrumMPI')
    os.system(f'sh loader.sh')
    if flag == 'u':
          codefile = codedir + '_unoptimized'
    elif flag == 'o':
        codefile = codedir + '_optimized'
    else:
         print('Command line format:\n\t[Program, u for unoptimized, o for optimized]')
         exit(1)
    os.system(f'mpixlc -o {codedir}/{codefile}.o {codedir}/{codefile}.c')
    return codefile

def make_script(program_dir, codefile):
        with open(f"{program_dir}/mpijob.lsf", "w") as f:
            header = 'source /polusfs/setenv/setup.SMPI\n' + \
                f'#BSUB -n 32 -q extended\n' + \
                '#BSUB -W 12:00\n' + \
                f'#BSUB -o {codefile}.out\n' + \
                f'#BSUB -e {codefile}.err\n\n'
            
            f.write(header)

            i = 1
            while i < MAX_PROCESSES+1:
                #f.write(f'echo "============ PROCESSES: {i} ============""\n')
                mpi_str = f'mpiexec -n {i} ./{program_dir}/{codefile}.o\n'
                i *= 2
                f.write(mpi_str)
                #f.write("\n")

def run_script(codefile):
    program_dir = codefile.split("_")[0]
    os.system(f'bsub < {program_dir}/mpijob.lsf')

if __name__ == "__main__":
    if len(argv) == 3:
        program_dir = str(argv[1])
        codefile = compile(program_dir, argv[2])
        make_script(program_dir, codefile)
        run_script(codefile)
    else:
        print('Command line format:\n\t[Program, u for unoptimized, o for optimized]')
        
