import { Student } from './student.model';

export class LastWork {
    id : number;
    is_updateable : boolean;
    status : string;
    consegna_id : string;
    studentId: string
    studentName: string
    studentSurname: string
    date : Date;
    lode : boolean;
    voto : number;
  
    constructor(id: number, is_updateable: boolean, status: string, date: Date, consegna_id : string, studentId: string, studentName: string, studentSurname: string, lode: boolean, voto: number){
        this.id = id;
        this.is_updateable = is_updateable;
        this.status = status;
        this.date = date;
        this.consegna_id = consegna_id;
        this.studentId = studentId;
        this.studentName = studentName;
        this.studentSurname = studentSurname;
        this.lode = lode;
        this.voto = voto;
    }
}