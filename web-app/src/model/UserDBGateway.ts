import { User } from '@/model/User'
import { SearchUserData } from '@/model/SearchUserData'
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
    const userDataJsonText = await response.text()
    const userDataJsonObject = JSON.parse(userDataJsonText, this.AvailConverter)
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

  AvailConverter(key: string, value: string) {
    if (key !== 'avail') {
      return value
    }
    if (value === '0') {
      return false
    } else {
      return true
    }
  }

  public async sendSearchUserData(searchUserData: SearchUserData): Promise<User[]> {
    const method = 'POST'
    const sendSearchUserDataJson = JSON.stringify({
      method: 'search',
      name: searchUserData.name,
      avail: searchUserData.avail,
      level: searchUserData.level
    })
    let response: Response
    try {
      response = await fetch(this.cgiUrl, {
        method: method,
        headers: {
          'Content-Type': 'application/json'
        },
        body: sendSearchUserDataJson
      })
    } catch (error) {
      alert(error)
      return []
    }
    if (response.status === 404) {
      alert('404 NotFound\nCGIが存在しません')
      return []
    }
    if (response.status === 200) {
      const userDataJson = await response.text()
      const userDataJsonObject = JSON.parse(userDataJson, this.AvailConverter)
      const userData: User[] = userDataJsonObject.users
      if (userData.length === 0) {
        alert('アカウントが見つかりませんでした')
        return []
      }
      // 一番最初に指定された条件でDBに検索をかける。
      // ２つ目以降の条件はクライアント側で処理をする。
      if (searchUserData.name !== '') {
        // 一番最初の条件が名前の場合
        const searchedUserData = userData
          .filter(x => {
            if (searchUserData.avail !== 'null') {
              if (searchUserData.avail === 'true') {
                return x.avail === true
              } else {
                return x.avail === false
              }
            }
            return true
          })
          .filter(x => {
            if (searchUserData.level !== 'null') {
              return x.level === searchUserData.level
            }
            return true
          })
        if (searchedUserData.length === 0) {
          alert('アカウントが見つかりませんでした')
          return []
        }
        return searchedUserData
      } else if (searchUserData.avail !== 'null') {
        // 一番最初の条件が状態の場合
        const searchedUserData = userData.filter(x => {
          if (searchUserData.level !== 'null') {
            return x.level === searchUserData.level
          }
          return true
        })
        if (searchedUserData.length === 0) {
          alert('アカウントが見つかりませんでした')
          return []
        }
        return searchedUserData
      } else {
        // 一番最初の条件が権限の場合
        // DB側で検索が終了しているためそのままデータを返す。
        return userData
      }
    }
    const text = await response.text()
    alert('検索に失敗しました。\n' + text)
    return []
  }

  public async sendFormUpdateUser(updatedUser: User): Promise<void> {
    const method = 'POST'
    const sendUpdateJson = JSON.stringify({
      method: 'update',
      id: updatedUser.id,
      name: updatedUser.name,
      pass: updatedUser.pass,
      avail: updatedUser.avail.toString(),
      level: updatedUser.level
    })
    let response: Response
    try {
      response = await fetch(this.cgiUrl, {
        method: method,
        headers: {
          'Content-Type': 'application/json'
        },
        body: sendUpdateJson
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
        alert('更新に失敗しました。\n' + text)
        return
    }
  }
}
