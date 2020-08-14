export class Profile{

    email: string
    name: string
    surname: string
    serial: string

    constructor(email: string, name: string, surname: string, serial: string){
        this.email = email
        this.name = name
        this.surname = surname
        this.serial = serial
    }
}