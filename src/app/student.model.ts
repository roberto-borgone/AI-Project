export class Student{

    id: number
    serial: string
    name: string
    firstName: string
    courseId: number
    group: string
    

    constructor(id: number, serial: string, name: string, firstName: string, courseId: number = 0, group: string){
        this.id = id
        this.serial = serial
        this.name = name
        this.firstName = firstName
        this.courseId = courseId
        this.group = group
    }
}