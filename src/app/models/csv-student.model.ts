export class CsvStudent{

    id: string
    name: string
    surname: string
    email: string
    done: boolean
    message: string
    

    constructor(id: string, name: string, surname: string, email: string, done : boolean, message: string){
        this.id = id
        this.name = name
        this.surname = surname
        this.email = email
        this.done = done
        this.message = message
    }
}