<template>
  <div id="app">
    <header id="main-header">
      <h1 id="header-title">{{ title }}</h1>
      <div id="header-button-aria">
        <button type="button" class="header-button" @click="$modal.show('add-user-form-modal')">
          アカウントを追加
        </button>
        <button type="button" class="header-button" @click="$modal.show('search-user-form-modal')">
          検索
        </button>
        <button type="button" class="header-button remove-button" @click="sendFormRemoveUser">
          選択したアカウントを削除
        </button>
      </div>
    </header>
    <main>
      <AddUserFormModal @submit="sendFormAddUser" />
      <SearchUserFormModal @submit="sendFormSearchUserData" />
      <UpdateUserFormModal :OriginalUser="updatedUser" @submit="sendFormUpdateUser" />
      <div id="userInfo-data-aria">
        <p>表示数：{{ userInfo.length }}</p>
        <div v-if="SearchConditionsMessageIsNotEmpty" id="search-conditions">
          検索条件：{{ SearchConditionsMessage }}アカウント
          <button id="reset-search-conditions-button" @click="reloadTable">
            検索条件をリセットする
          </button>
        </div>
      </div>
      <UserInfoTable
        :userInfo="userInfo"
        @selectedChanged="selectedChange"
        @update="openUpdateUserFormModal"
      />
    </main>
  </div>
</template>

<script lang="ts">
import { Component, Vue } from 'vue-property-decorator'
import UserInfoTable from '@/components/UserInfoTable.vue'
import AddUserFormModal from '@/components/AddUserFormModal.vue'
import SearchUserFormModal from '@/components/SearchUserFormModal.vue'
import UpdateUserFormModal from '@/components/UpdateUserFormModal.vue'
import { User } from '@/model/User'
import { UserDBGateway } from '@/model/UserDBGateway'
import { SearchUserData } from '@/model/SearchUserData'
import { AvailToPrintString } from '@/model/PrintString'

@Component({
  components: {
    UserInfoTable,
    AddUserFormModal,
    SearchUserFormModal,
    UpdateUserFormModal
  }
})
export default class App extends Vue {
  public title = 'UserDBAccesser'
  public gateway = new UserDBGateway()
  public userInfo: User[] = []
  public selectedIds: number[] = []

  public updatedUser?: User = {
    id: -1,
    name: '',
    pass: '',
    avail: true,
    level: 'ADMIN'
  }

  public SearchConditionsMessage?: string = ''

  public get SearchConditionsMessageIsNotEmpty(): boolean {
    return this.SearchConditionsMessage !== undefined && this.SearchConditionsMessage.length !== 0
  }

  private SearchUserDataToString(searchUserData: SearchUserData): string {
    let message = ''
    if (searchUserData.name.length !== 0) {
      message += ` 「${searchUserData.name}」を含む `
    }
    if (searchUserData.avail !== 'null') {
      const availString = AvailToPrintString(searchUserData.avail)
      message += ` ${availString}な `
    }
    if (searchUserData.level !== 'null') {
      message += ` ${searchUserData.level} `
    }
    if (message.length === 0) {
      return ''
    }
    return message
  }

  public async sendFormUpdateUser(updatedUser: User): Promise<void> {
    await this.gateway.sendFormUpdateUser(updatedUser)
    this.reloadTable()
  }

  public async reloadTable(): Promise<void> {
    const data = await this.gateway.getUserDataAsync()
    this.userInfo = data
    this.SearchConditionsMessage = ''
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

  public async sendFormSearchUserData(searchUserData: SearchUserData) {
    const searchedUsers = await this.gateway.sendSearchUserData(searchUserData)
    // 検索結果があれば反映
    if (searchedUsers.length !== 0) {
      this.userInfo = searchedUsers
      this.SearchConditionsMessage = this.SearchUserDataToString(searchUserData)
    }
  }

  public async openUpdateUserFormModal(updatedUser: User): Promise<void> {
    this.updatedUser = updatedUser
    this.$modal.show('update-user-form-modal')
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
  margin: 5px;
  background-color: white;
  padding: 10px;
  border: solid 5px #4472c4;
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
}

.remove-button {
  background-color: #ed7d31;
  border-color: #ae5a21;
}

.remove-button:active {
  border-color: #ed7d31;
  background-color: #eb965e;
}

.v--modal {
  border-radius: 20px !important;
}

#userInfo-data-aria {
  display: flex;
  font-size: 1.2em;
  margin-left: 10px;
}

#search-conditions {
  border: solid #2f528f 2px;
  padding: 0.1em 0.2em;
  margin: 0px 0.2em;
  border-radius: 10px;
  font-size: 0.7em;
}

button {
  color: #ffffff;
  background-color: #4472c4;
  border-color: #2f528f;
  border-radius: 5px;
}

button:active {
  background-color: #5b81c4;
  border-color: #4472c4;
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
  padding: 0px 10px;
}
</style>
