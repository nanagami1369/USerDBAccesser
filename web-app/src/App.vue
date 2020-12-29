<template>
  <div id="app">
    <header id="main-header" class="content">
      <h1 id="header-title">{{ title }}</h1>
      <div id="header-button-aria">
        <button type="button" class="header-button" @click="$modal.show('add-user-form-modal')">
          アカウントを追加
        </button>
        <button type="button" class="header-button">更新</button>
        <button type="button" class="header-button remove-button" @click="sendFormRemoveUser">
          選択したアカウントを削除
        </button>
      </div>
    </header>
    <main>
      <AddUserFormModal @submit="sendFormAddUser" />
      <UserInfoTable :userInfo="userInfo" @selectedChanged="selectedChange" />
    </main>
  </div>
</template>

<script lang="ts">
import { Component, Vue } from 'vue-property-decorator'
import UserInfoTable from '@/components/UserInfoTable.vue'
import AddUserFormModal from '@/components/AddUserFormModal.vue'
import { User } from '@/model/User'
import { UserDBGateway } from '@/model/UserDBGateway'

@Component({
  components: {
    UserInfoTable,
    AddUserFormModal
  }
})
export default class App extends Vue {
  public title = 'UserDBAccesser'
  public gateway = new UserDBGateway()
  public userInfo: User[] = []
  public selectedIds: number[] = []
  public async reloadTable(): Promise<void> {
    const data = await this.gateway.getUserDataAsync()
    this.userInfo = data
  }
  public async sendFormAddUser(addFormUser: User): Promise<void> {
    await this.gateway.sendFormAddUser(addFormUser)
    await this.reloadTable()
  }

  public async sendFormRemoveUser(): Promise<void> {
    if (this.selectedIds.length === 0) {
      alert('アカウントが選択されていません')
      return
    }
    let message = '以下のアカウントを削除します\nよろしいですか？\n\n'
    this.selectedIds.forEach(selectedId => {
      const user = this.userInfo.find(x => x.id === selectedId)
      if (user !== undefined) {
        message += `id : ${user.id} , name : ${user.name}\n`
      }
    })
    if (confirm(message)) {
      await this.gateway.sendFormRemoveUsers(this.selectedIds)
      await this.reloadTable()
    }
  }

  public selectedChange(selectedIds: number[]): void {
    this.selectedIds = selectedIds
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
.table-number {
  text-align: right;
}

.table-other-content {
  text-align: center;
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

.v--modal {
  border-radius: 20px !important;
}

table {
  margin-top: 10px;
  width: 100%;
  border-collapse: collapse;
}

table th {
  background-color: #4472c4;
  color: #eeeeee;
  padding: 2px;
}

table td {
  background-color: #cfd5ea;
}

table th,
table td {
  border: solid 1px #ffffff;
}
</style>
