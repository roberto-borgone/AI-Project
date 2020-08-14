export class User{
    
    // email field has to be removed in the end. It's just to make login work with mock server
    username: string
    password: string
    email:string

    constructor(username: string, password: string){
        this.username = username
        this.email = username
        this.password = password
    }
}