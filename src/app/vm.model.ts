import { Student } from './student.model'

export class VM{

    id: number
    ram: number 
    disk: number
    virtualCpu: number
    on: boolean
    path: string
    teamID: number
    owners: Student[] 
    

    constructor(id: number, ram: number, disk: number, virtualCpu: number, isOn: boolean, path: string, teamID: number, owners: Student[]){
        this.id = id
        this.ram = ram
        this.disk = disk
        this.virtualCpu = virtualCpu
        this.on = isOn
        this.path = path
        this.teamID = teamID
        this.owners = owners
    }
}