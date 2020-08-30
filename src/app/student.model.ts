export class Student{

    id: string
    name: string
    surname: string
    courseId: string
    group: string
    

    constructor(id: string, name: string, surname: string, courseId: string = '', group: string){
        this.id = id
        this.name = name
        this.surname = surname
        this.courseId = courseId
        this.group = group
    }
}