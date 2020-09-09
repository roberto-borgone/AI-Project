export class Team{

    id: number
    name: string
    maxRAM: number 
    maxDisk: number
    maxVCPU: number
    maxActiveVM: number
    maxTotVM: number
    

    constructor(id: number, name: string, maxRAM: number, maxDisk: number, maxVCPU: number, maxActiveVM: number, maxTotVM: number){
        this.id = id
        this.name = name
        this.maxRAM = maxRAM
        this.maxDisk = maxDisk
        this.maxVCPU = maxVCPU
        this.maxActiveVM = maxActiveVM
        this.maxTotVM = maxTotVM
    }
}