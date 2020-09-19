export class Assignment {

    id : number;
    rilascio: Date;
    scadenza: Date;
    courseName: string;
    docentID: string;
    status: string;

    constructor(id: number, rilascio: Date, scadenza: Date, courseName: string, docentID: string, status: string){
        this.id = id;
        this.rilascio = rilascio;
        this.scadenza = scadenza;
        this.courseName = courseName;
        this.docentID = docentID;
        this.status = status;
    }
}