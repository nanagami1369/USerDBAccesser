<template>
  <div id="app">
    <header id="main-header" class="content">
      <h1>{{ title }}</h1>
    </header>
    <main>
      <AddUserForm @submit="sendFormAddUser" />
      <UserInfoTable :userInfo="userInfo"></UserInfoTable>
    </main>
  </div>
</template>

<script lang="ts">
import { Component, Vue } from 'vue-property-decorator'
import UserInfoTable from '@/components/UserInfoTable.vue'
import AddUserForm from '@/components/AddUserForm.vue'
import { User } from '@/model/user'

@Component({
  components: {
    UserInfoTable,
    AddUserForm
  }
})
export default class App extends Vue {
  public title = 'UserDBAccesser'
  public readonly cgiUrl = '/cgi-bin/UserDBAccesser.cgi'
  public userInfo: User[] = []

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
  public async reloadTable(): Promise<void> {
    const data = await this.getUserDataAsync()
    this.userInfo = data
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
        await this.reloadTable()
        return
      default:
        alert('追加に失敗しました。\n' + text)
        return
    }
  }

  public async created() {
    await this.reloadTable()
  }
}
</script>

<style>
* {
  margin: 0;
  padding: 0;
}

.content {
  margin: 5px;
  background-color: white;
  padding: 10px;
  border: solid 5px #4472c4;
}
</style>
