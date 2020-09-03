export class CsvStudent{

    id: string
    name: string
    surname: string
    email: string
    done: boolean
    

    constructor(id: string, name: string, surname: string, email: string, done : boolean){
        this.id = id
        this.name = name
        this.surname = surname
        this.email = email
        this.done = done
    }
}