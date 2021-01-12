<template>
  <table id="user-data-table">
    <thead>
      <th class="id-column">ID</th>
      <th class="name-column">名前</th>
      <th class="password-column">パスワード</th>
      <th class="avail-column">状態</th>
      <th class="level-column">権限</th>
      <th class="update-button-column">更新</th>
      <th><!--余白--></th>
    </thead>
    <tbody>
      <tr
        v-for="user in selectableUserInfo"
        :key="user.id"
        @click="selectedChange(user)"
        :class="{ selected: user.isSelected }"
      >
        <td class="table-number">{{ user.id }}</td>
        <td>{{ user.name }}</td>
        <td>*******</td>
        <td class="table-other-content">{{ user.avail | availToString }}</td>
        <td class="table-other-content">{{ user.level | levelToString }}</td>
        <td class="table-other-content">
          <button id="update-button" type="button" @click.stop="update(user)">更新</button>
        </td>
        <td><!--余白--></td>
      </tr>
    </tbody>
  </table>
</template>

<script lang="ts">
import { Component, Prop, Vue } from 'vue-property-decorator'
import { User, SelectableUser } from '@/model/User'
import { AvailToPrintString, LevelToPrintString } from '@/model/PrintString'

@Component({
  filters: {
    availToString: function(avail: boolean) {
      return AvailToPrintString(avail)
    },
    levelToString: function(level: string) {
      return LevelToPrintString(level)
    }
  }
})
export default class UserInfoTable extends Vue {
  @Prop()
  private selectableUserInfo!: SelectableUser[]

  public selectedChange(selectedUser: SelectableUser): void {
    selectedUser.isSelected = !selectedUser.isSelected
    this.$emit(
      'selectedChanged',
      this.selectableUserInfo.filter(x => x.isSelected)
    )
  }

  public update(updatedUser: User) {
    this.$emit('update', updatedUser)
    return
  }
}
</script>
<style scoped>
.id-column {
  width: 4rem;
}

.name-column {
  width: 30%;
}

.password-column {
  width: 7rem;
}

.avail-column {
  width: 3rem;
}

.level-column {
  width: 5rem;
}

.update-button-column {
  width: 7rem;
}

#update-button {
  flex-shrink: 0;
  font-size: 1.4em;
  padding: 0.2em 0.8em;
  margin: 0px 5px;
}

.selected td {
  background-color: orange;
}
</style>
