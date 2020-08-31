export class Profile{

    email: string
    name: string
    surname: string
    id: string

    constructor(email: string, name: string, surname: string, id: string){
        this.email = email
        this.name = name
        this.surname = surname
        this.id = id
    }
}