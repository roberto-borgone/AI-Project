import { Student } from './student.model'

export class Proposal{

    name: string
    creator: string
    proposals: Student[]
    accepted: Student[]
    

    constructor(name: string, creator: string, proposals: Student[], accepted: Student[]){
        this.name = name
        this.creator = creator
        this.proposals = proposals
        this.accepted = accepted
    }
}