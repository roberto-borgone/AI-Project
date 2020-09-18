export class RegistartionForm{
    
    email: string
    password: string
    passwordConfirm: string
    name: string
    surname: string
    serial: string

    constructor(email: string, password: string, passwordConfirm: string, name: string, surname: string, serial: string){
        this.email = email
        this.password = password
        this.passwordConfirm = passwordConfirm
        this.name = name
        this.surname = surname
        this.serial = serial
    }
}