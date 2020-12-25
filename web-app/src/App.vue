<template>
  <div id="app">
    <header id="main-header" class="content">
      <h1 id="header-title">{{ title }}</h1>
      <div id="header-button-aria">
        <button type="button" class="header-button">アカウントを追加</button>
        <button type="button" class="header-button">更新</button>
        <button type="button" class="header-button remove-button">削除</button>
      </div>
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
import { User } from '@/model/User'
import { UserDBGateway } from '@/model/UserDBGateway'

@Component({
  components: {
    UserInfoTable,
    AddUserForm
  }
})
export default class App extends Vue {
  public title = 'UserDBAccesser'
  public gateway = new UserDBGateway()
  public userInfo: User[] = []

  public async reloadTable(): Promise<void> {
    const data = await this.gateway.getUserDataAsync()
    this.userInfo = data
  }
  public async sendFormAddUser(addFormUser: User): Promise<void> {
    await this.gateway.sendFormAddUser(addFormUser)
    await this.reloadTable()
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

#main-header {
  display: flex;
  flex-flow: row nowrap;
  justify-content: space-between;
  align-items: center;
}

#header-title {
  flex-grow: 1;
  font-size: 2em;
  flex-shrink: 0;
}

#header-button-aria {
  flex-shrink: 0;
  flex-grow: 3;
  display: flex;
  flex-flow: row nowrap;
  justify-content: flex-end;
}

.header-button {
  flex-shrink: 0;
  font-size: 1.4em;
  padding: 0.2em 0.8em;
  margin: 0px 5px;
  color: #ffffff;
  background-color: #4472c4;
  border-color: #2f528f;
  border-radius: 5px;
}

.header-button:active {
  background-color: #5b81c4;
  border-color: #4472c4;
}

.remove-button {
  background-color: #ed7d31;
  border-color: #ae5a21;
}

.remove-button:active {
  border-color: #ed7d31;
  background-color: #eb965e;
}

.content {
  margin: 5px;
  background-color: white;
  padding: 10px;
  border: solid 5px #4472c4;
}
</style>
