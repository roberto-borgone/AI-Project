export class Course{

    id: number
    name: string
    path: string
    min: number
    max: number

    constructor(id: number, name: string, path: string, min: number, max: number){
        this.id = id
        this.name = name
        this.path = path
        this.min = min
        this.max = max
    }
}