<template>
  <div id="app">
    <header id="main-header" class="content">
      <h1>{{ title }}</h1>
    </header>
    <main>
      <form id="add-user-form">
        <h4>アカウントの追加</h4>
        <label>名前</label>
        <input
          type="text"
          name="name"
          id="add-user-input-name"
          v-model="addUserForm.name"
          required
        />
        <label>パスワード(8文字以上)</label>
        <input
          type="password"
          name="password"
          pattern=".{8,}"
          id="add-user-input-password"
          v-model="addUserForm.pass"
          required
        />
        <label>状態</label>
        <label for="avail-ragio-true">
          <input
            type="radio"
            name="avail"
            value="true"
            id="avail-ragio-true"
            v-model="addUserForm.avail"
          />有効
        </label>
        <label for="avail-ragio-false">
          <input
            type="radio"
            name="avail"
            value="false"
            id="avail-ragio-false"
            v-model="addUserForm.avail"
          />無効
        </label>
        <label>権限</label>
        <label for="level-radio-admin">
          <input
            type="radio"
            name="level"
            value="ADMIN"
            id="level-radio-admin"
            v-model="addUserForm.level"
          />管理者
        </label>
        <label for="level-radio-prem">
          <input
            type="radio"
            name="level"
            value="PREM"
            id="level-radio-prem"
            v-model="addUserForm.level"
          />プレミア
        </label>
        <label for="level-radio-gen">
          <input
            type="radio"
            name="level"
            value="GEN"
            id="level-radio-gen"
            v-model="addUserForm.level"
          />一般
        </label>
        <label for="level-radio-try">
          <input
            type="radio"
            name="level"
            value="TRY"
            id="level-radio-try"
            v-model="addUserForm.level"
          />お試し
        </label>
        <input
          id="send-form-add-user-button"
          type="button"
          @click="sendFormAddUser"
          value="アカウントを追加する"
        />
      </form>
      <UserInfoTable :userInfo="userInfo"></UserInfoTable>
    </main>
  </div>
</template>

<script lang="ts">
import { Component, Vue } from 'vue-property-decorator'
import UserInfoTable from '@/components/UserInfoTable.vue'
import { User } from '@/model/user'

@Component({
  components: {
    UserInfoTable
  }
})
export default class App extends Vue {
  public title = 'UserDBAccesser'
  public readonly cgiUrl = '/cgi-bin/UserDBAccesser.cgi'
  public userInfo: User[] = []
  public addUserForm: User = {
    id: -1,
    name: '',
    pass: '',
    avail: true,
    level: 'ADMIN'
  }

  public async getUserDataAsync(): Promise<User[]> {
    const method = 'post'

    const response = await fetch(this.cgiUrl, {
      method: method,
      headers: {
        'Content-Type': 'application/json'
      },
      body: JSON.stringify({ method: 'getAll' })
    })
    const userDataJsonObject = await response.json()
    const userData = userDataJsonObject.users
    return userData
  }
  public async reloadTable(): Promise<void> {
    const data = await this.getUserDataAsync()
    this.userInfo = data
  }
  public async sendFormAddUser(): Promise<void> {
    const method = 'post'
    const sendAddUserJson = JSON.stringify({
      method: 'add',
      name: this.addUserForm.name,
      pass: this.addUserForm.pass,
      avail: this.addUserForm.avail.toString(),
      level: this.addUserForm.level
    })
    const response = await fetch(this.cgiUrl, {
      method: method,
      headers: {
        'Content-Type': 'application/json'
      },
      body: sendAddUserJson
    })
    const text = await response.text()
    if (response.status == 201) {
      console.log(text)
    } else {
      alert('追加に失敗しました。\n' + text)
    }
    await this.reloadTable()
    return
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
