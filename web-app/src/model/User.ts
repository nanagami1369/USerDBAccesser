export interface User {
  id: number
  name: string
  pass: string
  avail: boolean
  level: string
}

export interface SelectableUser extends User {
  isSelected: boolean
}
