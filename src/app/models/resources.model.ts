export class Resources{

    ram: number 
    disk: number
    vcpu: number
    n_active: number
    n_tot: number

    constructor(ram: number, disk: number, vcpu: number, n_active: number, n_tot: number){
        this.ram = ram
        this.disk = disk
        this.vcpu = vcpu
        this.n_active = n_active
        this.n_tot = n_tot
    }
}