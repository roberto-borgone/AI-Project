export class Student{

    id: string
    name: string
    surname: string
    group: string
    email: string
    

    constructor(id: string, name: string, surname: string, group: string, email: string){
        this.id = id
        this.name = name
        this.surname = surname
        this.group = group
        this.email = email
    }
}