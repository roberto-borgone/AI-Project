export class Assignment {

    id : number;
    rilascio: Date;
    scadenza: Date;
    course_id: string;
    docent_id: string;

    constructor(id: number, rilascio: Date, scadenza: Date, course_id: string, docent_id: string){
        this.id = id;
        this.rilascio = rilascio;
        this.course_id = course_id;
        this.docent_id = docent_id;
    }
}