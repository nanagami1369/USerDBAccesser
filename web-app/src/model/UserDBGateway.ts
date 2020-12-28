import { User } from '@/model/User'

export class UserDBGateway {
  private readonly cgiUrl = '/cgi-bin/UserDBAccesser.cgi'

  public async getUserDataAsync(): Promise<User[]> {
    const method = 'post'
    let response: Response
    try {
      response = await fetch(this.cgiUrl, {
        method: method,
        headers: {
          'Content-Type': 'application/json'
        },
        body: JSON.stringify({ method: 'getAll' })
      })
    } catch (error) {
      alert(error)
      return []
    }
    if (response.status === 404) {
      alert('404 NotFound\nCGIが存在しません')
      return []
    }
    const userDataJsonObject = await response.json()
    const userData: User[] = userDataJsonObject.users
    return userData
  }

  public async sendFormAddUser(addFormUser: User): Promise<void> {
    const method = 'post'
    const sendAddUserJson = JSON.stringify({
      method: 'add',
      name: addFormUser.name,
      pass: addFormUser.pass,
      avail: addFormUser.avail.toString(),
      level: addFormUser.level
    })
    let response: Response
    try {
      response = await fetch(this.cgiUrl, {
        method: method,
        headers: {
          'Content-Type': 'application/json'
        },
        body: sendAddUserJson
      })
    } catch (error) {
      alert(error)
      return
    }
    const text = await response.text()
    switch (response.status) {
      case 404:
        alert('404 NotFound\nCGIが存在しません')
        return
      case 201:
        console.log(text)
        return
      default:
        alert('追加に失敗しました。\n' + text)
        return
    }
  }

  public async sendFormRemoveUsers(ids: number[]): Promise<void> {
    const method = 'delete'
    const sendRemoveUserJson = JSON.stringify({
      method: 'remove',
      ids: ids
    })
    let response: Response
    try {
      response = await fetch(this.cgiUrl, {
        method: method,
        headers: {
          'Content-Type': 'application/json'
        },
        body: sendRemoveUserJson
      })
    } catch (error) {
      alert(error)
      return
    }
    const text = await response.text()
    switch (response.status) {
      case 404:
        alert('404 NotFound\nCGIが存在しません')
        return
      case 200:
        console.log(text)
        return
      default:
        alert('削除に失敗しました。\n' + text)
        return
    }
  }
}
