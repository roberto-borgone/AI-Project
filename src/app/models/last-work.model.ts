import { Student } from './student.model';

export class LastWork {

    id : number;
    updateable : boolean;
    status : string;
    consegnaId : number;
    studentId: string
    studentName: string
    studentSurname: string
    date : Date;
    lode : boolean;
    voto : number;
  
    constructor(id: number, updateable: boolean, status: string, date: Date, consegnaId : number, studentId: string, studentName: string, studentSurname: string, lode: boolean, voto: number){
        this.id = id;
        this.updateable = updateable;
        this.status = status;
        this.date = date;
        this.consegnaId = consegnaId;
        this.studentId = studentId;
        this.studentName = studentName;
        this.studentSurname = studentSurname;
        this.lode = lode;
        this.voto = voto;
    }
}