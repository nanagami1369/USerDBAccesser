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

.content {
  margin: 5px;
  background-color: white;
  padding: 10px;
  border: solid 5px #4472c4;
}
</style>
