import { Student } from './student.model';

export class LastWork {
    id : number;
    is_updateable : boolean;
    status : string;
    timestamp : Date;
    consegna_id : string;
    student : Student;
    date : Date;
    lode : boolean;
    voto : number;
  
    constructor(id: number, is_updateable: boolean, status: string, timestamp: Date, consegna_id : string, student: Student, lode: boolean, voto: number){
        this.id = id;
        this.is_updateable = is_updateable;
        this.status = status;
        this.timestamp = timestamp;
        this.consegna_id = consegna_id;
        this.student = student;
        this.lode = lode;
        this.voto = voto;
    }
}