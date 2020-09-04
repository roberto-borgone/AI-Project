export class Course{

    name: string
    aconimo: string
    min: number
    max: number
    enabled: boolean

    maxVirtualCPU: number
    maxRAM: number
    maxDisk: number
    maxActiveVM: number 
    maxTotVM: number

    constructor(name: string, acronimo: string, min: number, max: number, enabled: boolean){
        this.name = name
        this.aconimo = acronimo
        this.min = min
        this.max = max
        this.enabled = enabled
        this.maxVirtualCPU = -1
        this.maxRAM = -1
        this.maxDisk = -1
        this.maxActiveVM = -1 
        this.maxTotVM = -1
    }
}