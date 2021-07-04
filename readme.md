# MTech-project
Minimizes the energy consumption in a Cloud Enviornment by optimizing the allocation of Containers to Vms, and Vms to Pms.

## How to run
Either compile each library file individually and link them to main.c on execution or run `project-bpp.cbp` with `Code Blocks` and build from there.

## Configuration
The cofigurations can be found in `macros.c`. VM and container configuration can be selected from these:
- `VM_TYPE` : Denotes the type of VM to be selected, it can have `vm_syn` for synthetic vms and `vm_rea` for real vms.
- `VM_SIZE` : 10 for synthentic vms, 20 for real vms.
- `CNT_TYPE` : Denotes the type of containers to be allocated, it can have `cn_syn` for synthentic containers and `cn_rea`for real containers.
- `CNT_SIZE` : It can take values from `200,500,1000,1500`