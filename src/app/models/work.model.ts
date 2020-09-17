export class Work {
    id : number;
    timestamp : Date;
    consegna_id : number;
    student_id : string;
    type : string;

    constructor(id: number, timestamp: Date, consegna_id: number, student_id: string, type: string) {
        this.id = id;
        this.timestamp = timestamp;
        this.consegna_id = consegna_id;
        this.student_id = student_id;
        this.type = type;
    }
}